# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

import relay_server_pb2 as relay__server__pb2


class RelayServerStub(object):
  """The greeting service definition.
  """

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.get_state = channel.unary_unary(
        '/py_server.RelayServer/get_state',
        request_serializer=relay__server__pb2.Data.SerializeToString,
        response_deserializer=relay__server__pb2.State.FromString,
        )
    self.set_state = channel.unary_unary(
        '/py_server.RelayServer/set_state',
        request_serializer=relay__server__pb2.Data.SerializeToString,
        response_deserializer=relay__server__pb2.Response.FromString,
        )


class RelayServerServicer(object):
  """The greeting service definition.
  """

  def get_state(self, request, context):
    """gets state of appliance
    """
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')

  def set_state(self, request, context):
    """sets state of appliance
    """
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_RelayServerServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'get_state': grpc.unary_unary_rpc_method_handler(
          servicer.get_state,
          request_deserializer=relay__server__pb2.Data.FromString,
          response_serializer=relay__server__pb2.State.SerializeToString,
      ),
      'set_state': grpc.unary_unary_rpc_method_handler(
          servicer.set_state,
          request_deserializer=relay__server__pb2.Data.FromString,
          response_serializer=relay__server__pb2.Response.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'py_server.RelayServer', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
